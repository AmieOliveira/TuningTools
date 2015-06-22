#
# Installs boost_1_58_0. It will take a while, only source this if you are sure
# that your system does not contains the boost needed version already installed.
#

# FIXME:
# - We may want this to check if the boost is already installed in
# standard places and contains the python library. 
# - We would also want this to install only the boost python library. 

test "x$1" = "x" -o "x$2" = "x" && echo "$0: Wrong number of arguments" && exit 1

if ! `root-config --cxx` $2 -P boost_test.h
then
  NEW_ENV_FILE=$1
  echo "It is needed to install boost python library." 
  BOOST_LOCAL_PATH=$PWD
  test ! -f boost_1_58_0.tar.gz && wget http://sourceforge.net/projects/boost/files/boost/1.58.0/boost_1_58_0.tar.gz
  tar xfz boost_1_58_0.tar.gz
  cd boost_1_58_0
  ./bootstrap.sh --prefix=$BOOST_LOCAL_PATH --with-libraries=python 
  ./b2 install --prefix=$BOOST_LOCAL_PATH --with-python -j$ROOTCORE_NCPUS
  cd -
  boost_include=$BOOST_LOCAL_PATH/include
  boost_lib=$BOOST_LOCAL_PATH/lib
  echo "test \"\${PATH#*$boost_include}\" = \"\${PATH}\" && export PATH=$boost_include:\$PATH" >> $NEW_ENV_FILE
  echo "test \"\${LD_LIBRARY_PATH#*$boost_lib}\" = \"\${LD_LIBRARY_PATH}\" && export LD_LIBRARY_PATH=$boost_lib:\$LD_LIBRARY_PATH" >> $NEW_ENV_FILE
  test source $NEW_ENV_FILE && echo "Couldn't set environment" && exit 1
  test ! `root-config --cxx` $2 -P boost_test.h && echo "Couldn't install boost" && exit 1
else
  echo "Boost needed libraries already installed."
fi
