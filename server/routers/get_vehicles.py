from models import Vehicles
from database import SessionLocal
from fastapi import APIRouter, HTTPException
from sqlalchemy.exc import SQLAlchemyError


router = APIRouter()

@router.get("/get_vehicles")
async def get_vehicles():
    
    session = SessionLocal()

    try:
        # Query and return a list of all the vehicles in the database
        vehicles = session.query(Vehicles).all()

        return vehicles

    except SQLAlchemyError as e:
        raise HTTPException(status_code=500, detail=f"Database error: {str(e)}")

    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Unexpected error: {str(e)}")

    finally:
        session.close()