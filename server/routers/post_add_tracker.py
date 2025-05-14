from models import Trackers, Vehicles
from schemas import TrackerSchema
from database import SessionLocal
from fastapi import APIRouter, HTTPException
from sqlalchemy.exc import SQLAlchemyError


router = APIRouter()

@router.post("/post_add_tracker")
async def post_add_tracker(payload: TrackerSchema):
    
    session = SessionLocal()

    tracker_data = Trackers(
        tracker_id = payload.tracker_id,
        tracker_type = payload.tracker_type,
        tracker_on_site = True,
        tracker_battery_low = False
    )

    try:
        session.add(tracker_data)
        session.commit()
        return {"message": "Tracker data inserted successfully."}
        
    except SQLAlchemyError as e:
        session.rollback()
        raise HTTPException(status_code=500, detail=f"Database error: {str(e)}")

    except Exception as e:
        session.rollback()
        raise HTTPException(status_code=500, detail=f"Unexpected error: {str(e)}")
    
    finally:
        session.close()
