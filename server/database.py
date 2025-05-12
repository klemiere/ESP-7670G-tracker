import json
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker

with open('secrets.json') as f:
    secrets = json.load(f)

DATABASE_URL = f"mysql+mysqlconnector://{secrets['db_user']}:{secrets['db_password']}@localhost/clarebout"

engine = create_engine(DATABASE_URL)
SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)