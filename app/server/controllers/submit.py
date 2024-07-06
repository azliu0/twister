"""."""

import hashlib

from apiflask import APIBlueprint
from flask import jsonify, request
from server.config import SECRET_KEY

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

    user_id = body.get("user_id", None)
    submission = body.get("submission", None)

    if submission == "TODO(flag)":
        return jsonify(
            {
                "solved": True,
                "message": f"congrats! submit this flag to the command center to "
                f"collect your points: {_get_flag(user_id)}",
            }
        ), 200

    return jsonify({"solved": False, "message": "wrong"}), 400
