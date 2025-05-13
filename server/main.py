from fastapi import FastAPI
from fastapi.responses import RedirectResponse
from routers.get_coordinates_single_tracker import router as get_coordinates_single_tracker_router
from routers.get_coordinates_all_trackers import router as get_coordinates_all_trackers_router
from routers.get_coordinates_time_range import router as get_coordinates_time_range_router
from routers.get_user import router as get_user_router
from routers.post_coordinates import router as post_coordinates_router
from routers.post_user import router as post_new_user_router

server = FastAPI()

@server.get("/", include_in_schema=False)
async def root():
    return RedirectResponse(url="/docs")

server.include_router(get_coordinates_single_tracker_router)
server.include_router(get_coordinates_all_trackers_router)
server.include_router(get_coordinates_time_range_router)
server.include_router(get_user_router)
server.include_router(post_coordinates_router)
server.include_router(post_new_user_router)
