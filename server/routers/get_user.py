from models import Users
from database import SessionLocal
from fastapi import APIRouter, Query, HTTPException
from sqlalchemy.exc import SQLAlchemyError


router = APIRouter()

@router.get("/get_user")
async def get_user(
    username: str = Query(..., title="Username"),
    password: str = Query(..., title="Password")):
    
    session = SessionLocal()

    try:
        # Query users and find matching username and password
        user = session.query(Users).filter(Users.username == username,
                                           Users.password == password
                                           ).first()

        if not user:
            return {"message": "Incorrect password or user not found."}
        
        return {"message": "OK"}

    except SQLAlchemyError as e:
        raise HTTPException(status_code=500, detail=f"Database error: {str(e)}")

    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Unexpected error: {str(e)}")

    finally:
        session.close()
