#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/adam/catkin_ws_projekt/src/joint_state_publisher/joint_state_publisher"

# ensure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/adam/catkin_ws_projekt/install/lib/python2.7/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/adam/catkin_ws_projekt/install/lib/python2.7/dist-packages:/home/adam/catkin_ws_projekt/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/adam/catkin_ws_projekt/build" \
    "/usr/bin/python2" \
    "/home/adam/catkin_ws_projekt/src/joint_state_publisher/joint_state_publisher/setup.py" \
    egg_info --egg-base /home/adam/catkin_ws_projekt/build/joint_state_publisher/joint_state_publisher \
    build --build-base "/home/adam/catkin_ws_projekt/build/joint_state_publisher/joint_state_publisher" \
    install \
    --root="${DESTDIR-/}" \
    --install-layout=deb --prefix="/home/adam/catkin_ws_projekt/install" --install-scripts="/home/adam/catkin_ws_projekt/install/bin"
