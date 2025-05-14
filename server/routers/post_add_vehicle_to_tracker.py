from models import Trackers, Vehicles
from schemas import AddVehicleToTrackerSchema
from database import SessionLocal
from fastapi import APIRouter, HTTPException
from sqlalchemy.exc import SQLAlchemyError


router = APIRouter()

@router.post("/post_add_vehicle_to_tracker")
async def post_add_vehicle_to_tracker(payload: AddVehicleToTrackerSchema):
    
    session = SessionLocal()

    tracker = session.query(Trackers).filter(Trackers.tracker_identifier == payload.tracker_identifier).first()

    if tracker:
        vehicle = session.query(Vehicles).filter(Vehicles.vehicle_plate == payload.vehicle_plate).first()
        if vehicle:
            tracker.vehicle_id = vehicle.vehicle_id

    try:
        session.commit()
        return {"message": "Tracker and vehicle association successful."}
        
    except SQLAlchemyError as e:
        session.rollback()
        raise HTTPException(status_code=500, detail=f"Database error: {str(e)}")

    except Exception as e:
        session.rollback()
        raise HTTPException(status_code=500, detail=f"Unexpected error: {str(e)}")
    
    finally:
        session.close()
