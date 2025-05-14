from pydantic import BaseModel
from typing import Optional
from decimal import Decimal
from models import TrackerTypeEnum

class CoordinatesSchema(BaseModel):
    tracker_identifier: str
    timestamp: str
    latitude: Decimal
    longitude: Decimal

class UserSchema(BaseModel):
    username: str
    password: str

class TrackerSchema(BaseModel):
    tracker_identifier: str
    tracker_type: TrackerTypeEnum

class TrackerResponse(BaseModel):
    tracker_id: int
    tracker_identifier: str
    tracker_type: TrackerTypeEnum
    tracker_on_site: bool
    tracker_battery_low: bool
    vehicle_id: Optional[int] = None

    class Config:
        orm_mode = True

class VehicleResponse(BaseModel):
    vehicle_id: int
    vehicle_plate: str

    class Config:
        orm_mode = True


class AddVehicleToTrackerSchema(BaseModel):
    tracker_identifier: str
    vehicle_plate: str