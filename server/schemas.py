from pydantic import BaseModel
from decimal import Decimal

class Coordinates(BaseModel):
    tracker_id: int
    latitude: Decimal
    longitude: Decimal
    timestamp: str