

filename <- "CallPriceBounds.csv"
foldername <- "/Users/swatimital/GitHub/QuantPricer/Results/"
price.bounds.file <- paste(foldername, filename, sep="")
data <- read.csv(price.bounds.file, TRUE, sep=",")
stock_prices <- as.vector(t(data[,1]))
line.col <- c('red', 'green', 'blue', 'purple', 'pink', 'black')

par(lwd=2,cex=1.0)
plot(data[,1], data[,2], type='l', lty=2, col=line.col[1], ylim = c(-4,15), xlab='Stock Prices', ylab='Call Spread Prices', main='Bull Call Spread Prices')
#axis(side=1, at=seq(0, 200, by=20.0), pos=-0.1)
#axis(side=2, at=seq(-4, 15, by=2.0))

lines(data[,1], data[,3], pch=22, lty=2, col=line.col[3])
lines(data[,1], data[,4], pch=22, lty=2, col=line.col[4])
lines(data[,1], data[,5], col=line.col[4])
lines(data[,1], data[,6], col=line.col[5])
lines(data[,1], data[,7], col=line.col[6])

legend('topright', c('BS UB', 'BS LB', 'BS Mid', 'BSB UB', 'BSB LB'), lty=c(3,3,3,1,1), bty='n', col=line.col)
