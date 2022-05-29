sh ./test.sh 50 50 matrix 2 2 3
sh ./test.sh 50 50 matrix 3 4 5
sh ./test.sh 50 50 matrix 5 7 9
sh ./test.sh 50 50 matrix_no_reduction 2 2 3
sh ./test.sh 50 50 matrix_no_reduction 3 4 5
sh ./test.sh 50 50 matrix_no_reduction 5 7 9
sh ./test.sh 50 50 integration 100000
sh ./test.sh 50 50 integration 1000000
sh ./test.sh 50 50 integration 10000000
sh ./test.sh 50 50 integration_no_reduction 100000
sh ./test.sh 50 50 integration_no_reduction 1000000
sh ./test.sh 50 50 integration_no_reduction 10000000
sh ./test.sh 50 50 integration 100000000
sh ./test.sh 50 50 integration_no_reduction 100000000
sh ./test.sh 50 50 matrix 20 25 40
sh ./test.sh 50 50 matrix_no_reduction 20 25 40
sh ./test.sh 50 50 matrix 100 150 200
sh ./test.sh 50 50 matrix_no_reduction 100 150 200
sh ./test.sh 10 50 matrix 1000 1500 2000
sh ./test.sh 10 50 matrix_no_reduction 1000 1500 2000
sh ./test_scheduling.sh 50 50 matrix_scheduling 2 2 3
sh ./test_scheduling.sh 50 50 matrix_scheduling 3 4 5
sh ./test_scheduling.sh 50 50 matrix_scheduling 5 7 9
sh ./test_scheduling.sh 50 50 integration_scheduling 100000
sh ./test_scheduling.sh 50 50 integration_scheduling 1000000
sh ./test_scheduling.sh 50 50 integration_scheduling 10000000
sh ./test_scheduling.sh 50 50 integration_scheduling 100000000
sh ./test_scheduling.sh 50 50 matrix_scheduling 20 25 40
sh ./test_scheduling.sh 50 50 matrix_scheduling 100 150 200
sh ./test_scheduling.sh 10 50 matrix_scheduling 1000 1500 2000