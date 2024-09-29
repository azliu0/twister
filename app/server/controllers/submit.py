"""."""

import hashlib

from apiflask import APIBlueprint
from flask import jsonify, request
from server.config import FLAG, SECRET_KEY

submit = APIBlueprint("submit", __name__, url_prefix="/api/submit", tag="Submit")


def _get_flag(user_id: str) -> str:
    """Get the flag for a user."""
    return hashlib.sha256(f"{user_id}_{SECRET_KEY}".encode("utf-8")).hexdigest()  # noqa: UP012


@submit.post("/")
def submit_puzzle():
    """Submit endpoint."""
    body = request.json
    if body is None:
        return jsonify({"solved": False, "message": "Invalid request"}), 400

    user_id = body.get("user", None)
    flag = body.get("flag", None)

    if user_id is None or flag is None:
        return jsonify({"solved": False, "message": "Invalid request"}), 400

    # this kinda defeats the purpose of the user hashing but it's fine
    if flag == FLAG:
        return jsonify(
            {
                "solved": True,
                "message": f"congrats! submit this code to the command center to "
                f"collect your points: {_get_flag(user_id)}",
            }
        ), 200

    return jsonify({"solved": False, "message": "wrong"}), 400
