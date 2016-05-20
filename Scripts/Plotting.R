
plotBSBCurves <- function()
{
  filename <- "CallPriceBounds.csv"
  foldername <- "/Users/swatimital/GitHub/QuantPricer/Results/"
  price.bounds.file <- paste(foldername, filename, sep="")
  data <- read.csv(price.bounds.file, TRUE, sep=",")
  stock_prices <- as.vector(t(data[,1]))
  stock_prices
  line.col <- sample(colours(), ncol(data)-1)
  plot(data[,1], data[,2], type='l', col=line.col[1], xaxt='n', xlab='Stock Prices', ylab='Call Spread Prices', ylim=c(-3.5, 15))
  axis(side=1, stock_prices, at=stock_prices)
  for (i in 3:ncol(data))
  {
    lines(data[,1], data[,i], col=line.col[i-1])
  }
}