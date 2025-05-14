from models import Trackers
from schemas import TrackerResponse
from database import SessionLocal
from fastapi import APIRouter, Query, HTTPException
from typing import List
from sqlalchemy.exc import SQLAlchemyError


router = APIRouter()

@router.get("/get_trackers", response_model=List[TrackerResponse])
async def get_trackers(
    tracker_id: int = Query(0, title="Tracker ID")):
    
    session = SessionLocal()

    try:
        # If tracker_id = 0 return all trackers
        if tracker_id == 0:
            trackers = session.query(Trackers).all()

            return trackers
        
        else:
            tracker = session.query(Trackers).filter(Trackers.tracker_id == tracker_id).first()

            if not tracker:
                return {"message": "No tracker with this ID found."}
            
            return tracker

    except SQLAlchemyError as e:
        raise HTTPException(status_code=500, detail=f"Database error: {str(e)}")

    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Unexpected error: {str(e)}")

    finally:
        session.close()
