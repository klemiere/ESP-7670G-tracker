import enum
from sqlalchemy import Column, Integer, Numeric, TIMESTAMP, String, Boolean, Enum, ForeignKey
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import relationship

Base = declarative_base()

class Users(Base):
    __tablename__ = 'users'

    user_id = Column(Integer, primary_key=True, nullable=False)
    user_username = Column(String(24), nullable=False)
    user_password = Column(String(180), nullable=False)

class Vehicles(Base):
    __tablename__ = 'vehicles'

    vehicle_id = Column(Integer, primary_key=True, autoincrement=True)
    vehicle_plate = Column(String(20), nullable=False)

    trackers = relationship("Trackers", back_populates="vehicle")

class TrackerTypeEnum(enum.Enum):
    lte = "lte"
    lora = "lora"

class Trackers(Base):
    __tablename__ = 'trackers'

    tracker_id = Column(Integer, primary_key=True, autoincrement=True)
    tracker_identifier = Column(String(20), nullable=False)
    tracker_type = Column(Enum(TrackerTypeEnum), nullable=False)
    tracker_on_site = Column(Boolean, default=True, nullable=False)
    tracker_battery_low = Column(Boolean, default=False, nullable=False)
    vehicle_id = Column(Integer, ForeignKey('vehicles.vehicle_id'))

    vehicle = relationship("Vehicles", back_populates="trackers")

class Positions(Base):
    __tablename__ = 'positions'

    position_id = Column(Integer, primary_key=True, autoincrement=True)
    position_timestamp = Column(TIMESTAMP)
    position_lat = Column(Numeric, nullable=False)
    position_long = Column(Numeric, nullable=False)
    tracker_id = Column(Integer, ForeignKey('trackers.tracker_id'))