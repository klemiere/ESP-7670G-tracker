from models import Position
from schemas import Coordinates
from database import SessionLocal
from fastapi import APIRouter, HTTPException
from sqlalchemy.exc import SQLAlchemyError


router = APIRouter()

@router.post("/post_coordinates")
async def post_request(payload: Coordinates):
    
    session = SessionLocal()

    position_data = Position(
        position_timestamp = payload.timestamp,
        position_lat = payload.latitude,
        position_long = payload.longitude,
        tracker_id=payload.tracker_id
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

