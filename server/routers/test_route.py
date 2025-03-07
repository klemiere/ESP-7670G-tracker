from fastapi import APIRouter
from pydantic import BaseModel

router = APIRouter()

class Message(BaseModel):
    Msg: str

@router.get("/test_route")
async def test():
    print("Works")
    return "Hello"