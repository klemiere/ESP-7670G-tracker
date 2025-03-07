from fastapi import APIRouter
from pydantic import BaseModel

router = APIRouter()

class Message(BaseModel):
    Msg: str

@router.post("/test_route")
async def create_post(post: Message):
    return Message