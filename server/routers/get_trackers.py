from database import SessionLocal
from models import Trackers
from schemas import TrackerResponse
from converters import to_tracker_response
from fastapi import APIRouter, Query, HTTPException
from typing import List
from sqlalchemy.exc import SQLAlchemyError
from sqlalchemy.orm import joinedload


router = APIRouter()

@router.get("/get_trackers", response_model=List[TrackerResponse])
async def get_trackers(
    tracker_identifier: str = Query("", title="Tracker identifier")):
    
    session = SessionLocal()

    try:
        # If tracker_identifier is empty, assume all trackers must be returned
        if tracker_identifier == "":
            trackers = session.query(Trackers).options(joinedload(Trackers.vehicle)).all()
            return [to_tracker_response(t) for t in trackers]
        
        else:
            tracker = session.query(Trackers)\
                .filter(Trackers.tracker_identifier == tracker_identifier)\
                .options(joinedload(Trackers.vehicle))\
                .first()

            if not tracker:
                raise HTTPException(status_code=404, detail="No tracker with this identifier found.")
            
            return [to_tracker_response(tracker)]

    except HTTPException as e:
        raise e
    
    except SQLAlchemyError as e:
        raise HTTPException(status_code=500, detail=f"Database error: {str(e)}")

    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Unexpected error: {str(e)}")

    finally:
        session.close()
