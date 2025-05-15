import random
import requests
from decimal import Decimal
from datetime import datetime, timedelta

positions = []

now = datetime.now()
counter = 1
for i in range(9):
    positions.append({
        'tracker_identifier': "testid1",
        'timestamp': (now + timedelta(hours=i)).strftime("%Y-%m-%d %H:%M:%S"),
        'latitude': float(random.uniform(50.0000, 52.0000)),
        'longitude': float(random.uniform(2.0000, 3.0000))
    })

for position in positions:
    resp = requests.post("http://localhost:8000/post_coordinates", json=position)
    print(resp.status_code, resp.json())