#!/bin/sh 
#chmod u+x

EXPECTED_ARGS=3

SAVE_IN="../tests/rand_GH"

if [ $# -lt $EXPECTED_ARGS ]
then
  echo "Usage: `basename $0` {argv}. <Loop repetitions for each G> <G probability(<1)> <H probability>"
  exit $E_BADARGS
fi

NUM_REP_G="$1"
G_prob="$2"
H_prob="$3"


if [ ! -d $SAVE_IN ]; then
  echo "Folder $SAVE_IN do not exist"
  exit $E_BADARGS
fi

#graphs_g=(6 7 8 9 10 11 12 13 14 15 20 25 30)
graphs_g=(8 9 10 11 12 13)

# make sure to have the right binary files 
make clean
make

# for each desired size of G
for g_size in "${graphs_g[@]}";
do
  new_folder="$SAVE_IN/G_$g_size"
  if [ ! -d $new_folder ]; then
    mkdir $new_folder;
  fi

  for((g=1; g<=$NUM_REP_G; g++));
  do
    folder_2save="$new_folder/test_$g"
    if [ ! -d $folder_2save ]; then
      mkdir $folder_2save;
    fi

    make G_H numVG=$g_size probG=$G_prob probH=$H_prob
    cp "../etc/list_file_out.txt" "$folder_2save/"
    cp "../etc/graph_H_out.txt" "$folder_2save/"
    cp "../etc/graph_G_out.txt" "$folder_2save/"

    make run G="$folder_2save/graph_G_out.txt" H="$folder_2save/graph_H_out.txt" L="$folder_2save/list_file_out.txt" > "$folder_2save/results.txt"
  done

done
