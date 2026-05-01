try {

    var currentTime = new Date();
    var threshold = 0.5;

    // Initialize on first run
    if (me.prev_x === undefined) {
        me.prev_x = me.x_value || 0;
        me.prev_y = me.y_value || 0;
        me.prev_z = me.z_value || 0;
        me.LastMoveTime = currentTime;
    }

    // Calculate change
    var dx = Math.abs((me.x_value || 0) - me.prev_x);
    var dy = Math.abs((me.y_value || 0) - me.prev_y);
    var dz = Math.abs((me.z_value || 0) - me.prev_z);

    // Movement detection
    var isMoving = (dx > threshold || dy > threshold || dz > threshold);

    // Time difference
    var lastTime = me.LastMoveTime || currentTime;
    var diff = (currentTime.getTime() - new Date(lastTime).getTime()) / 1000;

    if (isMoving) {
        me.LastMoveTime = currentTime;
        me.NoMovementAlert = false;
    } else {
        me.NoMovementAlert = (diff > 30);
    }

    // Update previous values
    me.prev_x = me.x_value || 0;
    me.prev_y = me.y_value || 0;
    me.prev_z = me.z_value || 0;

    // Alert message
    me.AlertMessage = me.NoMovementAlert 
        ? "🔴 NO MOVEMENT ALERT" 
        : "🟢 NORMAL";

} catch (e) {
    logger.warn("Error in motion detection logic: " + e);
}