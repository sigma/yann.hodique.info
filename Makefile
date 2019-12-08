.PHONY: build-prod
build-prod:
	hugo --gc --minify -b $(URL)

.PHONY: build-pre
build-pre:
	hugo --gc --minify -b $(DEPLOY_PRIME_URL)
