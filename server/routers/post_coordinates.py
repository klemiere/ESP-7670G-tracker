from fastapi import APIRouter
from pydantic import BaseModel

router = APIRouter()

class Coordinates(BaseModel):
    latitude: str
    longitude: str
    time: str
    date: str

@router.post("/post_coordinates")
async def create_post(post: Coordinates):
    return Coordinates