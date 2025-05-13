from models import Position
from database import SessionLocal
from fastapi import APIRouter, Query, HTTPException
from sqlalchemy.exc import SQLAlchemyError


router = APIRouter()

@router.get("/get_coordinates_single_tracker")
async def get_positions_single_tracker(
    tracker_id: int = Query(..., title="tracker_id",
                            description="ID of the tracker to query for."), 
    limit: int = Query(1, title="limit",
                       description="Amount of records to return.")):
    
    session = SessionLocal()

    try:
        # Query positions, sort by timestamp descending, retrieve a "limit" amount of datapoints and put them in a "position" list
        positions = session.query(Position).filter(Position.tracker_id == tracker_id).order_by(Position.position_timestamp.desc()).limit(limit).all()

        if not positions:
            return {"message": "No position found for the given tracker ID."}
        
        return positions

    except SQLAlchemyError as e:
        raise HTTPException(status_code=500, detail=f"Database error: {str(e)}")

    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Unexpected error: {str(e)}")

    finally:
        session.close()
