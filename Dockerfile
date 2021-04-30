FROM ubuntu:21.04 
#we need 21.04 for a newer cmake version

#in case our build halts
ENV TZ=Europe/Belgrade
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone


RUN apt-get update -y && \
		apt-get install -y build-essential cmake qtbase5-dev qtmultimedia5-dev qtdeclarative5-dev libqt5svg5-dev libkf5i18n-dev libkf5filemetadata-dev baloo-kf5-dev libkf5config-dev libkf5kcmutils-dev libkf5declarative-dev libkf5coreaddons-dev libkf5doctools-dev libkf5xmlgui-dev libkf5crash-dev libkf5dbusaddons-dev extra-cmake-modules gettext qml-module-qt-labs-platform qml-module-qtqml-models2 qml-module-org-kde-kcm qml-module-qtquick-controls qml-module-qtquick-dialogs qml-module-qtquick-layouts qml-module-qtquick2 qtquickcontrols2-5-dev libvlc-dev kirigami2-dev


COPY . /usr/src/elisa-debug

WORKDIR /usr/src/elisa-debug 

RUN mkdir build && \
		echo "Creating build directory for Elisa: " && \
		ls -la && \
		echo "Changing to build directory: " && \
		cd build && \
		echo "building project with cmake: " && \
		cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug && \
		echo "Listing make options." && \
		make help && \
		echo "Building executables." && \
		make elisa && \
   	echo "Build finished... Finally :) " 

CMD ["./build/bin/elisa"]
