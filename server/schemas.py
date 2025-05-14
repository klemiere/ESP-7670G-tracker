from models import TrackerTypeEnum
from pydantic import BaseModel, field_serializer
from typing import Optional
from datetime import datetime
from decimal import Decimal

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

class CoordinatesResponse(BaseModel):
    tracker_id: int
    position_id: int
    position_timestamp: datetime
    position_lat: Decimal
    position_long: Decimal

    model_config = {
        "from_attributes": True
    }

    #Remove the T from the datetime object for consistency
    @field_serializer("position_timestamp")
    def serialize_timestamp(self, dt: datetime, _info):
        return dt.strftime("%Y-%m-%d %H:%M:%S")

class TrackerResponse(BaseModel):
    tracker_id: int
    tracker_identifier: str
    tracker_type: TrackerTypeEnum
    tracker_on_site: bool
    tracker_battery_low: bool
    vehicle_id: Optional[int] = None

    model_config = {
        "from_attributes": True
    }

class VehicleResponse(BaseModel):
    vehicle_id: int
    vehicle_plate: str

    model_config = {
        "from_attributes": True
    }


class AddVehicleToTrackerSchema(BaseModel):
    tracker_identifier: str
    vehicle_plate: str