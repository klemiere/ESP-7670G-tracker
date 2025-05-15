from models import Trackers
from schemas import TrackerResponse

def to_tracker_response(tracker: Trackers) -> TrackerResponse:
    return TrackerResponse(
        tracker_id=tracker.tracker_id,
        tracker_identifier=tracker.tracker_identifier,
        tracker_type=tracker.tracker_type,
        tracker_on_site=tracker.tracker_on_site,
        tracker_battery_low=tracker.tracker_battery_low,
        vehicle_plate=tracker.vehicle.vehicle_plate if tracker.vehicle else None
    )