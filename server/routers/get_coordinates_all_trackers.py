from schemas import CoordinatesResponse
from models import Positions
from database import SessionLocal
from converters import to_coordinates_response
from fastapi import APIRouter, Query, HTTPException
from typing import List
from sqlalchemy import func
from sqlalchemy.exc import SQLAlchemyError
from sqlalchemy.orm import aliased


router = APIRouter()

@router.get("/get_coordinates_all_trackers", response_model=List[CoordinatesResponse])
async def get_positions_all_trackers(limit: int = Query(1, title="limit",
                       description="Amount of records to return.")):
    
    session = SessionLocal()
    
    try:
        # Add a window function for row numbering
        row_number_column = func.row_number().over(
            partition_by=Positions.tracker_id,
            order_by=Positions.position_timestamp.desc()
        ).label("row_number")

        # Create a subquery with the row number
        subquery = (
            session.query(Positions, row_number_column)
            .subquery()
        )

        # Alias the subquery so we can refer to Position columns cleanly
        SubPosition = aliased(Positions, subquery)

        # Query only rows where row_number <= limit
        results = (
            session.query(SubPosition)
            .filter(subquery.c.row_number <= limit)
            .all()
        )

        if not results:
            raise HTTPException(status_code=404, detail="No positions found.")
        
        return to_coordinates_response(results)
    
    except HTTPException as e:
        raise

    except SQLAlchemyError as e:
        raise HTTPException(status_code=500, detail=f"Database error: {str(e)}")

    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Unexpected error: {str(e)}")

    finally:
        session.close()