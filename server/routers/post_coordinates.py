from models import Positions, Trackers
from schemas import CoordinatesSchema
from database import SessionLocal
from fastapi import APIRouter, HTTPException
from sqlalchemy.exc import SQLAlchemyError


router = APIRouter()

@router.post("/post_coordinates")
async def post_request(payload: CoordinatesSchema):
    
    session = SessionLocal()

    tracker = session.query(Trackers).filter(Trackers.tracker_identifier == payload.tracker_identifier).first()
    
    position_data = Positions(
        tracker_id = tracker.tracker_id,
        position_timestamp = payload.timestamp,
        position_lat = payload.latitude,
        position_long = payload.longitude
    )

    try:
        session.add(position_data)
        session.commit()
        return {"message": "Position data inserted successfully."}
        
    except SQLAlchemyError as e:
        session.rollback()
        raise HTTPException(status_code=500, detail=f"Database error: {str(e)}")

    except Exception as e:
        session.rollback()
        raise HTTPException(status_code=500, detail=f"Unexpected error: {str(e)}")
    
    finally:
        session.close()

