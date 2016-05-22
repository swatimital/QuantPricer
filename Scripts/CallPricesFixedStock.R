

filename <- "CallPricesFixedStock.csv"
foldername <- "/Users/swatimital/GitHub/QuantPricer/Results/"
call.prices.file <- paste(foldername, filename, sep="")
data <- read.csv(call.prices.file, TRUE, sep=",")

par(lwd=2,cex=1.0, bty='n')
plot(data[,1], data[,2], type='l', lty=2, col=line.col[1], xlab='Strike Prices', ylab='Call Prices', main='Call Prices')

