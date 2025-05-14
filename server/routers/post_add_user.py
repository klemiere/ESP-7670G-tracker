from models import Users
from schemas import UserSchema
from database import SessionLocal
from fastapi import APIRouter, HTTPException
from sqlalchemy.exc import SQLAlchemyError


router = APIRouter()

@router.post("/post_new_user")
async def post_new_user(payload: UserSchema):
    
    session = SessionLocal()

    user_data = Users(
        username = payload.username,
        password = payload.password
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
