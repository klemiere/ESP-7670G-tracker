from models import Trackers
from schemas import TrackerResponse
from database import SessionLocal
from fastapi import APIRouter, Query, HTTPException
from typing import List
from sqlalchemy.exc import SQLAlchemyError


router = APIRouter()

@router.get("/get_trackers", response_model=List[TrackerResponse])
async def get_trackers(
    tracker_identifier: str = Query("", title="Tracker identifier")):
    
    session = SessionLocal()

    try:
        # If tracker_identifier is empty, assume all trackers must be returned
        if tracker_identifier == "":
            trackers = session.query(Trackers).all()

            return trackers
        
        else:
            tracker = session.query(Trackers).filter(Trackers.tracker_identifier == tracker_identifier).first()

            if not tracker:
                raise HTTPException(status_code=404, detail="No tracker with this identifier found.")
            
            return [tracker]

    except HTTPException as e:
        raise e
    
    except SQLAlchemyError as e:
        raise HTTPException(status_code=500, detail=f"Database error: {str(e)}")

    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Unexpected error: {str(e)}")

    finally:
        session.close()
