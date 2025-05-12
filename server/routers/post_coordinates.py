from fastapi import APIRouter
from pydantic import BaseModel

router = APIRouter()

class Coordinates(BaseModel):
    latitude: str
    longitude: str
    datetime: str
    vehicule: str

@router.post("/post_coordinates")
async def create_post(payload: Coordinates):
    print(payload)
    return payload