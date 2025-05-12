from fastapi import FastAPI
from fastapi.responses import RedirectResponse
from routers.post_coordinates import router as post_coordinates_router
from routers.test_route import router as test_route_router

server = FastAPI()

@server.get("/", include_in_schema=False)
async def root():
    return RedirectResponse(url="/docs")

server.include_router(post_coordinates_router)
server.include_router(test_route_router)