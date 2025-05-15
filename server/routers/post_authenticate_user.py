from models import Users
from schemas import UserSchema
from database import SessionLocal
import bcrypt
from fastapi import APIRouter, Query, HTTPException
from sqlalchemy.exc import SQLAlchemyError


router = APIRouter()

@router.post("/post_authenticate_user")
async def get_user(payload: UserSchema):
    
    session = SessionLocal()

    try:
        # Query users and find matching username and password
        user = session.query(Users).filter(Users.user_username == payload.username).first()

        if not user or not (bcrypt.checkpw(payload.password.encode(), user.user_password.encode())):
            raise HTTPException(status_code=404, detail="Invalid credentials")
            
        
        # Managing auth (or most of this API really) is NOT part of my tasks so I'm not making it any more complicated than this
        return {"message": "OK"}

    except HTTPException as e:
        raise e

    except SQLAlchemyError as e:
        raise HTTPException(status_code=500, detail=f"Database error: {str(e)}")

    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Unexpected error: {str(e)}")

    finally:
        session.close()
