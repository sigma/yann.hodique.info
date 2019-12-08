# change static/_redirects when increasing version
functions/vanity_v1: src/vanity/vanity.go deps
	mkdir -p functions
	go build -o $@ -ldflags "-X main.user=sigma -X main.vanity=yrh.dev" $<

.PHONY: deps
deps:
	go get ./src/...

.PHONY: functions
functions: functions/vanity_v1

.PHONY: build-prod
build-prod: functions
	hugo --gc --minify -b $(URL)

.PHONY: build-pre
build-pre: functions
	hugo --gc --minify -b $(DEPLOY_PRIME_URL)
