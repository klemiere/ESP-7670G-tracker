from fastapi import FastAPI
from fastapi.responses import RedirectResponse
from routers.post_coordinates import router as post_coordinates_router
from routers.get_coordinates import router as get_coordinates_router
from routers.get_coordinates_time_range import router as get_coordinates_time_range_router

server = FastAPI()

@server.get("/", include_in_schema=False)
async def root():
    return RedirectResponse(url="/docs")

server.include_router(get_coordinates_router)
server.include_router(post_coordinates_router)
server.include_router(get_coordinates_time_range_router)
