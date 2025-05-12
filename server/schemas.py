from pydantic import BaseModel

class Coordinates(BaseModel):
    tracker_id: int
    latitude: float
    longitude: float
    timestamp: str
    vehicule: str