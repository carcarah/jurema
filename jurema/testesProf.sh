#!/bin/bash



#./programa && kill `pidof -o %PPID sleep` & 


# matar o sleep somente oprograma acabar antes dos 5 segundos, jah que liber ao shell e o sleep dormirá 5

#sleep 5  && kill `pidof -o %PPID programa` 

# Mata o processo que temnome de "programa" somente se o sleep dormir seus 5 e retornar true.Não libera o shell

echo "INICIO DOS TESTES"

time ./jurema.out -s -jp <  professor/centro1132S.mat > relatorios/centro1132.rep && kill `pidof -o %PPID sleep` &

sleep 10800 && kill `pidof -o %PPID jurema.out`

time ./jurema.out -s -jp <  professor/centro1138S.mat > relatorios/centro1138.rep && kill `pidof -o %PPID sleep` &

sleep 10800  && kill `pidof -o %PPID jurema.out`

time ./jurema.out -s -jp <  professor/centro1131S.mat > relatorios/centro1131.rep && kill `pidof -o %PPID sleep` &

sleep 10800  && kill `pidof -o %PPID jurema.out`


time ./jurema.out -s -jp <  professor/sul1121S.mat > relatorios/sul1121.rep && kill `pidof -o %PPID sleep` &

sleep 10800  && kill `pidof -o %PPID jurema.out`


time ./jurema.out -s -jp < professor/sul1117S.mat > relatorios/sul1117.rep && kill `pidof -o %PPID sleep` &

sleep 10800  && kill `pidof -o %PPID jurema.out`

time ./jurema.out -s -jp <  professor/aldeota7111S.mat > relatorios/aldeota7111.rep && kill `pidof -o %PPID sleep` &

sleep 10800  && kill `pidof -o %PPID jurema.out`

time ./jurema.out -s -jp <  professor/aldeota1111S.mat > relatorios/aldeota1111.rep && kill `pidof -o %PPID sleep` &

sleep 10800  && kill `pidof -o %PPID jurema.out`

time ./jurema.out -s -jp <  professor/sul1122S.mat > relatorios/sul1122.rep && kill `pidof -o %PPID sleep` &

sleep 10800  && kill `pidof -o %PPID jurema.out`

time ./jurema.out -s -jp <  professor/sul1106S.mat > relatorios/sul1106.rep && kill `pidof -o %PPID sleep` &

sleep 10800  && kill `pidof -o %PPID jurema.out`

time ./jurema.out -s -jp <  professor/centro1111S.mat > relatorios/centro1111.rep && kill `pidof -o %PPID sleep` &

sleep 10800  && kill `pidof -o %PPID jurema.out`

echo "TESTES FINALIZADOS"


 





