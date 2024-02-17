#!/usr/bin/env bash
PRACTICE=${2:-p2}

cd $PRACTICE
make > /dev/null 2>&1
if [ "$PRACTICE" = "p1" ]; then
  levels=(1324 1625 1854 2356 2856 3456 3552 3728 4356 4592)
  TABLE=${1:-false};
  if [ "$TABLE" = "false" ]; then
    for i in {1..9999};
    do
      point=$(../simulador/simulador -level $i | cut -f1)
      echo $i, $point
    done
  else
    for i in ${levels[@]};
    do
      point=$(../simulador/simulador -t 1000 -level $i | cut -f1)
      make convertimages > /dev/null 2>&1
      mv strategy.ppm.png "images/strategy$i.png" > /dev/null 2>&1
      echo $i, $point
    done
  fi
else
  levels=(3124 3125 3054 3056 3056 3056 3052 3028 4056 4092)
  pases=(0.5 0.6 0.4 0.5 0.7 1 0.5 0.6 0.3 0.5)
  defenses=(30 40 40 50 40 50 40 60 70 80)
  defensestypes=(30 40 30 40 20 50 40 50 70 70)
  randomness=(0.5 0.6 0.4 0.5 0.7 0.5 0.5 0.6 0.3 0.5)
  for i in "${!levels[@]}";
  do
    point=$(../simulador/simulador -time 1000 -level "${levels[$i]}" -pases "${pases[i]}" -defenses "${defenses[$i]}" -defensetypes "${defensestypes[$i]}" -dr "${randomness[$i]}")
    echo "${levels[$i]}", $point
  done
fi