# Breakpoint python script for worm070 and all later versions.
# Note: we use the local variable doLevel::userworm to model the worm

def breakpoint_callback(frame, bp_loc, dict):
    # Get the target object
    thread = frame.GetThread()
    target = frame.GetThread().GetProcess().GetTarget()

    # Obtain references to local variables, which model the worm
    # Find frame of function doLevel() and locate the local
    # variable userworm.
    doLevel_frame = None
    for i in range(0, thread.num_frames):
        if thread.GetFrameAtIndex(i).name == "doLevel":
            doLevel_frame = thread.GetFrameAtIndex(i)  

    headindex = doLevel_frame.EvaluateExpression("userworm.headindex").signed
    maxindex = doLevel_frame.EvaluateExpression("userworm.maxindex").signed
    cur_lastindex = doLevel_frame.EvaluateExpression("userworm.cur_lastindex").signed
    dx = doLevel_frame.EvaluateExpression("userworm.dx").signed
    dy = doLevel_frame.EvaluateExpression("userworm.dy").signed

    # Dump worm configuration
    template="\n\ndoLevel:userworm: headindex=%-3d maxindex=%-3d cur_lastindex=%-3d dx=%-2d dy=%-2d\n"
    print(template % (headindex, maxindex, cur_lastindex, dx, dy))

    print("  i:   y,   x")
    print("-------------")
    for i in range(0,cur_lastindex + 1):
        wormpos_x = doLevel_frame.EvaluateExpression("userworm.wormpos["+ str(i) + "].x").signed
        wormpos_y = doLevel_frame.EvaluateExpression("userworm.wormpos["+ str(i) + "].y").signed
        if headindex == i:
            print("%3d: %3d, %3d <-- headindex" % (i, wormpos_y, wormpos_x))
        else:
            print("%3d: %3d, %3d" % (i,  wormpos_y, wormpos_x))

    print("")
    return True

