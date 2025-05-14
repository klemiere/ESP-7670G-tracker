from database import SessionLocal
from models import Positions, Trackers
from fastapi import APIRouter, Query, HTTPException
from sqlalchemy.exc import SQLAlchemyError
from datetime import datetime


router = APIRouter()

@router.get("/get_coordinates_time_range")
async def get_positions_time_range(tracker_identifier: str = Query(..., title="Tracker identifier",
                                                           description="ID of the tracker to query."), 
                                    start_time: str = Query(..., title="start_time",
                                                            description="Oldest time: YYYY-MM-DD HH:MM:SS"),
                                    end_time: str = Query(..., title="end_time",
                                                          description="Latest time: YYYY-MM-DD HH:MM:SS")):
    
    session = SessionLocal()

    tracker = session.query(Trackers).filter(Trackers.tracker_identifier == tracker_identifier).first()
    if not tracker:
        raise HTTPException(status_code=404, detail=f"tracker: {tracker_identifier} not found.")
    
    try:
        # Parse the string timestamps to datetime objects
        start_dt = datetime.strptime(start_time, "%Y-%m-%d %H:%M:%S")
        end_dt = datetime.strptime(end_time, "%Y-%m-%d %H:%M:%S")

        # Query the positions between the timestamps
        positions = session.query(Positions).filter(
            Positions.tracker_id == tracker.tracker_id,
            Positions.position_timestamp >= start_dt,
            Positions.position_timestamp <= end_dt
        ).order_by(Positions.position_timestamp.desc()).all()

        if not positions:
            raise HTTPException(status_code=404, detail=f"No positions for {tracker_identifier} found")

        return positions

    except HTTPException as e:
        raise e
    
    except ValueError:
        raise HTTPException(status_code=400, detail="Invalid date format. Use 'YYYY-MM-DD HH:MM:SS'.")

    except SQLAlchemyError as e:
        raise HTTPException(status_code=500, detail=f"Database error: {str(e)}")

    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Unexpected error: {str(e)}")

    finally:
        session.close()
