# ========== Script properties ========== #

OUTPUT_DIR=results/
EXPECTED_DIR=tests/expected/
TEST_DIR=tests/
TEST=${TEST_DIR}M-01-96-N-ok.fir


TARGETS="xml asm"

COMPILER_EXT=".fir"
ERR_EXT=".err"
ASM_EXT=".asm"
EXPECTED_EXT=".expected"
OUTPUT_EXT=".out"
DIFF_EXT=".diff"
ELF_EXT=".o"

MAKE="make"
MAKECLEAN="make clean"

DIFF="diff"
MKDIR="mkdir"
RM="rm -r"
CP="cp"
LS="ls"

REPORT_FILE=${OUTPUT_DIR}"report.txt"


# ========== Directory generation and compilation ========== #

${RM} ${OUTPUT_DIR}
${MKDIR} ${OUTPUT_DIR}

for inputTest in `${LS} ${TEST_DIR}*${COMPILER_EXT}`
do
        testType=`echo ${inputTest} | cut -c7`
        ${MKDIR} -p ${OUTPUT_DIR}${testType}
done

${MAKECLEAN}
${MAKE}
makeRv=$?

if [ ${makeRv} != 0 ]; then
        echo "Failed to compile compiler."
        exit 1
fi


# ========== Tests execution and comparison ========== #

echo =========================================================== 
echo -------------Test Report------------- > ${REPORT_FILE}
echo "Test name     | Test Summary" >> ${REPORT_FILE}
#for inputTest in ${TEST}
for inputTest in `${LS} ${TEST_DIR}*${COMPILER_EXT} | sort -V`
do
        # ========== Test properties ==========
        testType=`echo ${inputTest} | cut -c7`
        testName=`basename ${inputTest} ${COMPILER_EXT}`
        testDir="${OUTPUT_DIR}${testType}/${testName}/"
        testBase=${testDir}${testName}
        testErr=${testBase}${ERR_EXT}

        # ========== Test directory setup ========== #
        ${MKDIR} ${testDir}
        ${CP} ${inputTest} ${testDir}
        ${CP} ${EXPECTED_DIR}${testName}${OUTPUT_EXT} ${testBase}${EXPECTED_EXT}

        # ========== Generation for each target available ========== #
        buildResult=""
        for target in ${TARGETS}
        do
                targetOut=${testBase}.${target}
                echo ----${target} >> ${testErr}
                echo Building ${testName} "for target" ${target}...
                ./fir -g ${inputTest} -o ${targetOut} --target ${target} 2>> ${testErr}
                rvBuild=$?
                if [ ${rvBuild} = 139 ]; then #E0157 deu segfault 127
                        buildResult="Segmentation fault building ${target}."
                        echo ${buildResult} >> ${testErr}
                fi
                echo Finished building ${testName} "for target" ${target}.
        done

        # ========== Final output of the fir source code ========== #
        targetAsm=${testBase}${ASM_EXT}
        targetOut=${testBase}${OUTPUT_EXT}
        targetElf=${testBase}${ELF_EXT}
        targetExe=${testBase}

        echo ----yasm >> ${testErr}
        yasm -felf32 -o ${targetElf} ${targetAsm} 2>> ${testErr}
        rvYasm=$?
        
        cat ${targetAsm} | grep ";" > ${targetAsm}legible
        
        echo ----ld >> ${testErr}
        ld -m elf_i386 -o ${targetExe} ${targetElf} -L$HOME/compiladores/root/usr/lib -lrts 2>> ${testErr}
        rvLd=$?
        
        echo ----exec >> ${testErr}
        ./${targetExe} > ${targetOut} 2>> ${testErr}
        rvExe=$?

        execResult=""
        if [ ${rvExe} = 1 ]; then
                execResult="Return value = 1 - invalid arguments (number or value)."
        elif [ ${rvExe} = 2 ]; then
                execResult="Return value = 2 - execution error."
        elif [ ${rvExe} -gt 128 ]; then
                execResult="Return value > 128 - program terminated with signal ${rvExe}."
        fi

        if [ ${rvExe} != 0 ]; then
                echo ${execResult} >> ${testErr}
        fi
                
        if [ ${rvYasm} = 0 -a ${rvLd} = 0 -a ${rvExe} = 0 ]; then
                rm ${testErr}
        fi

        # ========== Diff between output and expected ========== #
        targetExpected=${testBase}${EXPECTED_EXT}
        targetDiff=${testBase}${DIFF_EXT}
        ${DIFF} ${targetOut} ${targetExpected} > ${targetDiff}
        rvDiff=$?

        # ========== Find the result of the test execution ========== #
        diffResult=""
        if [ ${rvDiff} = 0 ]; then
                diffResult="No differences found."
                rm ${targetDiff}
        elif [ ${rvDiff} = 1 ]; then
                diffResult="Some differences found."
        else 
                diffResult="There was a problem comparing the output with the expected result..."
        fi

        result=""
        if [ ${rvBuild} != 0 ] ; then
                result=${buildResult}
        else
                result=${diffResult}
        fi

        echo Test Summary: ${result}

        # ========== Report fill up and division ========== #
        printf "%-12s\t %s" ${testName} >> ${REPORT_FILE}
        echo ${result} >> ${REPORT_FILE}
        echo =========================================================== 
done

echo -------------Overall------------- >> ${REPORT_FILE}
printf "Module\t\t| #tests\t| with errors\t| different\n" >> ${REPORT_FILE}
for testModule in $(find ${OUTPUT_DIR} -mindepth 1 -maxdepth 1 -type d | sort -V)
do
        printf "%-12s\t" ${testModule} >> ${REPORT_FILE}

        numModuleTests=$(find ${testModule} -mindepth 1 -maxdepth 1 -type d | wc -l)
        printf "  %d\t\t  %d\t\t  %d\n" ${numModuleTests} $(find ${testModule} -name *${ERR_EXT} -type f -print | wc -l) $(find ${testModule} -name *${DIFF_EXT} -type f -print | wc -l) >> ${REPORT_FILE}
done
${MAKECLEAN}
cat ${REPORT_FILE}