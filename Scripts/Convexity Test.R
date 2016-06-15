library(plot3D)
library(rgl)
BSPrice <- function(St, K, r, q, t, T, vol, call)
{
  e1 <- abs(St-K)
  e2 <- T-t
  
  d1 <- (log(St/K) + ((r-q) + vol*vol*0.5)*(T-t))/(vol*sqrt(T-t))
  d2 <- (log(St/K) + ((r-q) - vol*vol*0.5)*(T-t))/(vol*sqrt(T-t))
  
  if (e1 < 1e-9 && e2 < 1e-9)
  {
    d1 <- Inf
    d2 <- Inf
  }
  
  if (call == TRUE)
  {
    return (St*exp(-q*(T-t))*pnorm(d1) - K*exp(-r*(T-t))*pnorm(d2))
  }
  else
  {
    return(K*exp(-r*(T-t))*pnorm(-d2) - St*exp(-q*(T-t))*pnorm(-d1))
  }
}

Gamma <- function(St, K, r, q, t, T, vol)
{
  d1 <- (log(St/K) + (r - q + vol*vol*0.5)*(T-t))/(vol*sqrt(T-t))
  return (exp(-q*(T-t)) * exp(-d1*d1*0.5)/(sqrt(2*pi)*St*vol*sqrt(T-t)))
}

BSPrice(100.0, 100.0, 0.05, 0, 0, 1.0, 0.0, TRUE)
Gamma(100.0, 100.0, 0.05,0,  0, 1.0, 0.1)

max.price <- 500
maturity <- 20
K <- 350
rf <- 0.05
vol <- 0.1
q <- 0.0

K.low <- 250
K.high <- 300
T.low <- 20
T.high <- 20

stock.prices <- seq(5, max.price, by=5)
time <- seq(0, maturity, by=1)

list.prices <- c()
for (price in stock.prices)
{
  for (t in time)
  {
    list.prices <- c(list.prices, BSPrice(price, K.low, rf, q, t, T.high, vol, TRUE)-BSPrice(price, K.high, rf, q, t, T.low, vol, TRUE))
  }
}
prices <- matrix(list.prices, ncol=length(stock.prices), nrow=length(time))

open3d()
bg3d("white")
material3d(col = "black")
persp3d(x = time, y = stock.prices, z = prices, theta=60, xlab='time', ylab='Stock', zlab='Option Price', col=rainbow(1000))

list.gamma <- c()
for (price in stock.prices)
{
  for (t in time)
  {
    call.spread.gamma <- Gamma(price, K.low, rf, q, t, T.high, vol)-Gamma(price, K.high, rf, q, t, T.low, vol)
    if (call.spread.gamma < 1e-9)
      call.spread.gamma <- 0.0
    list.gamma <- c(list.gamma, call.spread.gamma)
  }
}
gammas <- matrix(list.gamma, ncol=length(stock.prices), nrow=length(time))
persp3D(x=time, y = stock.prices, z=gammas, theta=120, phi=10, xlab='time', ylab='Stock', zlab='Gamma')
plot(stock.prices, gammas[20,], type='l')



filename <- "CallSpreadGammas.csv"
foldername <- "/Users/swatimital/GitHub/QuantPricer/Results/"
write.table(gammas, file=paste(foldername, filename, sep=""), sep=",")

S0 <- 300
low.strikes <- seq(100,500,by=10)
list.prices <- c()
BSPrice(S0, 300, rf, q, maturity, maturity, vol, FALSE)
time <- seq(0, maturity, by=1)
for (strike in low.strikes)
{
  for (t in time)
  {
    #list.prices <- c(list.prices, BSPrice(S0, strike, rf, q, t, maturity, vol, TRUE)-
    #                   BSPrice(S0, strike + 50, rf, q, t, maturity, vol, TRUE))  
    list.prices <- c(list.prices, -BSPrice(S0, strike, rf, q, t, maturity, vol, FALSE))
  }
}

prices <- matrix(list.prices, ncol=length(low.strikes), nrow=length(time))

open3d()
bg3d("white")
material3d(col = "black")
persp3d(x = time, y = low.strikes, z = prices, xlab='time', ylab='Strike', zlab='Option Price', col=rainbow(1000))
plot(low.strikes, prices[18,], type='l')
lines(low.strikes, prices[2,], type='l')
lines(low.strikes, prices[3,], type='l')