from models import Position
from database import SessionLocal
from fastapi import APIRouter, Query, HTTPException
from sqlalchemy import func, and_
from sqlalchemy.exc import SQLAlchemyError
from sqlalchemy.orm import aliased


router = APIRouter()

@router.get("/get_coordinates_all_trackers")
async def get_positions_all_trackers(limit: int = Query(1, title="limit",
                       description="Amount of records to return.")):
    
    session = SessionLocal()
    
    try:
        # Add a window function for row numbering
        row_number_column = func.row_number().over(
            partition_by=Position.tracker_id,
            order_by=Position.position_timestamp.desc()
        ).label("row_number")

        # Create a subquery with the row number
        subquery = (
            session.query(Position, row_number_column)
            .subquery()
        )

        # Alias the subquery so we can refer to Position columns cleanly
        SubPosition = aliased(Position, subquery)

        # Query only rows where row_number <= limit
        results = (
            session.query(SubPosition)
            .filter(subquery.c.row_number <= limit)
            .all()
        )

        return results if results else {"message": "No positions found."}

    except SQLAlchemyError as e:
        raise HTTPException(status_code=500, detail=f"Database error: {str(e)}")

    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Unexpected error: {str(e)}")

    finally:
        session.close()