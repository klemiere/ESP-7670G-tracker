from models import Users
from schemas import UserSchema
from database import SessionLocal
import bcrypt
from fastapi import APIRouter, HTTPException
from sqlalchemy.exc import SQLAlchemyError


router = APIRouter()

@router.post("/post_new_user")
async def post_new_user(payload: UserSchema):
    
    session = SessionLocal()

    # encode password to bytes and hash it
    hashed_password = bcrypt.hashpw(payload.password.encode('utf-8'), bcrypt.gensalt())

    user_data = Users(
        user_username = payload.username,
        user_password = hashed_password.decode('utf-8') # decode into string
    )

    try:
        session.add(user_data)
        session.commit()
        return {"message": "User data inserted successfully."}
        
    except SQLAlchemyError as e:
        session.rollback()
        raise HTTPException(status_code=500, detail=f"Database error: {str(e)}")

    except Exception as e:
        session.rollback()
        raise HTTPException(status_code=500, detail=f"Unexpected error: {str(e)}")
    
    finally:
        session.close()
