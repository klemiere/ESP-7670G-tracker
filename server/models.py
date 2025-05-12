import enum
from sqlalchemy import Column, Integer, Numeric, TIMESTAMP, VARCHAR, Boolean, Enum, ForeignKey
from sqlalchemy.ext.declarative import declarative_base

Base = declarative_base()

class Users(Base):
    __tablename__ = 'users'

    id = Column(Integer, primary_key=True, nullable=False)
    username = Column(VARCHAR(4), nullable=False)
    password = Column(VARCHAR(180), nullable=False)

class Vehicle(Base):
    __tablename__ = 'vehicle'

    vehicle_id = Column(Integer, primary_key=True, autoincrement=True)
    vehicle_plate = Column(VARCHAR(20), nullable=False)

class TrackerTypeEnum(enum.Enum):
    gps = "gps"
    lora = "lora"

class Tracker(Base):
    __tablename__ = 'tracker'

    tracker_id = Column(Integer, primary_key=True, autoincrement=True)
    tracker_type = Column(Enum(TrackerTypeEnum), nullable=False)
    tracker_on_site = Column(Boolean, default=False, nullable=False)
    tracker_battery_low = Column(Boolean, default=False, nullable=False)
    vehicle_id = Column(Integer, ForeignKey('vehicle.vehicle_id'))

class Position(Base):
    __tablename__ = 'position'

    position_id = Column(Integer, primary_key=True, autoincrement=True)
    position_timestamp = Column(TIMESTAMP)
    position_lat = Column(Numeric, nullable=False)
    position_long = Column(Numeric, nullable=False)
    tracker_id = Column(Integer, ForeignKey('tracker.tracker_id'))