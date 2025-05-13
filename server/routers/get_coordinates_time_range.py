from datetime import datetime
from models import Position
from database import SessionLocal
from fastapi import APIRouter, Query, HTTPException
from sqlalchemy.exc import SQLAlchemyError


router = APIRouter()

@router.get("/get_coordinates_time_range")
async def get_positions_time_range(tracker_id: int = Query(..., title="tracker_id",
                                                           description="ID of the tracker to query."), 
                                    start_time: str = Query(..., title="start_time",
                                                            description="Oldest time: YYYY-MM-DD HH:MM:SS"),
                                    end_time: str = Query(..., title="end_time",
                                                          description="Latest time: YYYY-MM-DD HH:MM:SS")):
    
    session = SessionLocal()

    try:
        # Parse the string timestamps to datetime objects
        start_dt = datetime.strptime(start_time, "%Y-%m-%d %H:%M:%S")
        end_dt = datetime.strptime(end_time, "%Y-%m-%d %H:%M:%S")

        # Query the positions between the timestamps
        positions = session.query(Position).filter(
            Position.tracker_id == tracker_id,
            Position.position_timestamp >= start_dt,
            Position.position_timestamp <= end_dt
        ).order_by(Position.position_timestamp.desc()).all()

        if not positions:
            return {"message": "No position found in the given time range."}

        return positions

    except ValueError:
        raise HTTPException(status_code=400, detail="Invalid date format. Use 'YYYY-MM-DD HH:MM:SS'.")

    except SQLAlchemyError as e:
        raise HTTPException(status_code=500, detail=f"Database error: {str(e)}")

    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Unexpected error: {str(e)}")

    finally:
        session.close()
