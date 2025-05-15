from models import Trackers, Positions
from schemas import TrackerResponse, CoordinatesResponse

def to_tracker_response(tracker: Trackers) -> TrackerResponse:
    return TrackerResponse(
        tracker_id=tracker.tracker_id,
        tracker_identifier=tracker.tracker_identifier,
        tracker_type=tracker.tracker_type,
        tracker_on_site=tracker.tracker_on_site,
        tracker_battery_low=tracker.tracker_battery_low,
        vehicle_plate=tracker.vehicle.vehicle_plate if tracker.vehicle else None
    )

def to_coordinates_response(positions: list[Positions]) -> list[CoordinatesResponse]:
    return [
        CoordinatesResponse(
            position_id=pos.position_id,
            tracker_identifier=pos.tracker.tracker_identifier,
            position_timestamp=pos.position_timestamp,
            position_lat=pos.position_lat,
            position_long=pos.position_long
        )
        for pos in positions
    ]