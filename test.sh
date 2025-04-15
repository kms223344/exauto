#!/bin/bash

# 인수 확인
if [ $# -ne 1 ]; then
  echo "Usage: $0 <반복횟수>"
  exit 1
fi

REPEAT=$1

# 숫자인지 확인
if ! [[ "$REPEAT" =~ ^[0-9]+$ ]]; then
  echo "반복 횟수는 숫자여야 합니다."
  exit 1
fi

for ((i=1; i<=REPEAT; i++)); do
  SEED=$(date +%s%N)  # 밀리초 단위 시드
  INPUT=$(./genabc "$SEED")

  echo -ne "[Run $i] \tSeed: $SEED ? "

  RET1=$(echo "$INPUT" | ./test01; echo $?)
  RET2=$(echo "$INPUT" | ./test02; echo $?)

  if [ "$RET1" -ne "$RET2" ]; then
    echo "❌ MISMATCH on seed $SEED: test01=$RET1, test02=$RET2"
    echo "▶️ 실패한 입력:"
    echo "--------------------"
    echo "$INPUT"
    echo "--------------------"
    exit 1
  else
    echo "✅ PASS RET $RET1"
  fi

  # sleep 0.001  # 시드 중복 방지
done

echo "🎉 모든 $REPEAT개의 테스트가 통과했습니다!"

