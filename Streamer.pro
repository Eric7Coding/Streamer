#-------------------------------------------------
#
# Project created by QtCreator 2017-5-14T00:15:28
#
#-------------------------------------------------

# Qt sub dirs, a big program consisting of numbers of sub programs such as an application with some shared libraries
TEMPLATE    =   subdirs

# every sub program
SUBDIRS =   lib/rtmp \
            Log \
            Plugins \
            Streamer

# compile order, the order indicating in SUBDIRS
CONFIG  +=  ordered

# Input
