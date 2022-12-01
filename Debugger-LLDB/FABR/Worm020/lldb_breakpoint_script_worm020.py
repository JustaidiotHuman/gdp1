# Breakpoint python script for worm020
# Note: we use global variables to model the worm

def breakpoint_callback(frame, bp_loc, dict):
    # Get the target object
    target = frame.GetThread().GetProcess().GetTarget()

    # Obtain references to globals variables, which model the worm

    headindex = target.FindFirstGlobalVariable("theworm_headindex")
    maxindex = target.FindFirstGlobalVariable("theworm_maxindex")
    dx = target.FindFirstGlobalVariable("theworm_dx")
    dy = target.FindFirstGlobalVariable("theworm_dy")
    theworm_wormpos_x = target.FindFirstGlobalVariable("theworm_wormpos_x")
    theworm_wormpos_y = target.FindFirstGlobalVariable("theworm_wormpos_y")

    # Dump worm configuration

    template="\n\ntheworm: headindex=%-3d maxindex=%-3d dx=%-2d dy=%-2d\n"
    print(template % (headindex.signed, maxindex.signed, dx.signed, dy.signed))

    print("  i:   y,   x")
    print("-------------")
    for i in range(0,maxindex.signed + 1):
        if headindex.signed == i:
            print("%3d: %3d, %3d <-- headindex" % (i, theworm_wormpos_y.GetChildAtIndex(i).signed, theworm_wormpos_x.GetChildAtIndex(i).signed) )
        else:
            print("%3d: %3d, %3d" % (i, theworm_wormpos_y.GetChildAtIndex(i).signed, theworm_wormpos_x.GetChildAtIndex(i).signed) )

    print("")
    return True

