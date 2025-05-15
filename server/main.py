from fastapi import FastAPI
from fastapi.responses import RedirectResponse
from routers.get_coordinates_single_tracker import router as get_coordinates_single_tracker_router
from routers.get_coordinates_all_trackers import router as get_coordinates_all_trackers_router
from routers.get_coordinates_time_range import router as get_coordinates_time_range_router
from routers.post_check_user import router as post_check_user_router
from routers.get_trackers import router as get_trackers_router
from routers.get_vehicles import router as get_vehicles_routers
from routers.post_coordinates import router as post_coordinates_router
from routers.post_add_user import router as post_add_user_router
from routers.post_add_tracker import router as post_add_tracker_router
from routers.post_add_vehicle_to_tracker import router as post_add_vehicle_to_tracker_router

server = FastAPI()

@server.get("/", include_in_schema=False)
async def root():
    return RedirectResponse(url="/docs")

server.include_router(get_coordinates_single_tracker_router)
server.include_router(get_coordinates_all_trackers_router)
server.include_router(get_coordinates_time_range_router)
server.include_router(get_trackers_router)
server.include_router(get_vehicles_routers)
server.include_router(post_coordinates_router)
server.include_router(post_add_user_router)
server.include_router(post_check_user_router)
server.include_router(post_add_tracker_router)
server.include_router(post_add_vehicle_to_tracker_router)