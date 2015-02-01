#!/bin/sh
json2tab=../json2tab
junescape=../junescape
testdata=testdata.txt

tab=$(printf \\t)

cut_vars() {
    # Cuts key, type, value from the given argument
    local _rest
    key=${1%%$tab*}
    _rest=${1#*$tab}
    type=${_rest%$tab*}
    value=${_rest#*$tab}
}

last_person_id=

{ <$testdata $json2tab; echo /; } | while IFS= read -r line; do
    cut_vars "$line"

    person_id=${key#/}; person_id=${person_id%%/*}
    person_subkey=${key#/*/}; person_subkey=${person_subkey%%/*}

    if [ "$person_id" != "$last_person_id" ]; then
        [ -n "$last_person_id" ] && printf '%s has these friends: %s\n' "$person_name" "$friends"
        friends=
        last_person_id=$person_id
    fi

    case $person_subkey in
        friends) [ "${key#/*/*/*/}" = name ] && friends="${friends:+$friends, }$value";;
        name) person_name=$value;;
    esac
done
