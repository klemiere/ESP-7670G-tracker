from database import SessionLocal
from models import Positions, Trackers
from schemas import CoordinatesResponse
from fastapi import APIRouter, Query, HTTPException
from typing import List
from sqlalchemy.exc import SQLAlchemyError


router = APIRouter()

@router.get("/get_coordinates_single_tracker", response_model=List[CoordinatesResponse])
async def get_positions_single_tracker(
    tracker_identifier: str = Query(..., title="Tracker identifier",
                            description="The tracker's identifier"), 
    limit: int = Query(1, title="limit",
                       description="Amount of records to return."),):
    
    session = SessionLocal()

    tracker = session.query(Trackers).filter(Trackers.tracker_identifier == tracker_identifier).first()
    if not tracker:
        raise HTTPException(status_code=404, detail=f"tracker: {tracker_identifier} not found")

    try:
        # Query positions, sort by timestamp descending, retrieve a "limit" amount of datapoints and put them in a "position" list
        positions = session.query(Positions).filter(Positions.tracker_id == tracker.tracker_id).order_by(Positions.position_timestamp.desc()).limit(limit).all()

        if not positions:
            raise HTTPException(status_code=404, detail=f"No positions for {tracker_identifier} found")
        
        return positions
    
    except HTTPException as e:
        raise e

    except SQLAlchemyError as e:
        raise HTTPException(status_code=500, detail=f"Database error: {str(e)}")

    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Unexpected error: {str(e)}")

    finally:
        session.close()
