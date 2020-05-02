.phony: all

all: c++.html python.html julia.html

%.html: %.md
	pandoc \
		-f markdown+tex_math_single_backslash \
		-t revealjs \
		--katex \
		-s \
		-V revealjs-url=https://revealjs.com \
		-V theme=white \
		-V width=1440 \
		-V height=810 \
		-o $@ \
		$<
