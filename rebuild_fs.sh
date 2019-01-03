#!/bin/bash

SRC_FRONTEND_ROOT=src-frontend
FS_ROOT=fs

function compress_css_for_html ()
{
    purifycss "$SRC_FRONTEND_ROOT/$1" "$SRC_FRONTEND_ROOT/$2" --min true --out "$FS_ROOT/$1"
}

function compress_html ()
{
    html-minifier --collapse-inline-tag-whitespace --collapse-whitespace --remove-comments \
        --remove-attribute-quotes --remove-redundant-attributes --output "$FS_ROOT/$1" \
        "$SRC_FRONTEND_ROOT/$1"
}

function compress_js ()
{
    uglifyjs -c -m "toplevel=true,reserved=['configure_sta','configure_ap','disable_webui']" \
        -o "$FS_ROOT/$1" "$SRC_FRONTEND_ROOT/$1"
}

function make ()
{
    echo Compressing css...
    compress_css_for_html __mgwwc_lit.css __mgwwc_index.html
    compress_css_for_html __mgwwc_util.css __mgwwc_index.html

    echo Compressing js...
    compress_js __mgwwc_main.js

    echo Compressing html...
    compress_html __mgwwc_index.html

    echo Done.
}

make

