from pydantic import BaseModel
from decimal import Decimal
from enum import Enum

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
    tracker_type: str
