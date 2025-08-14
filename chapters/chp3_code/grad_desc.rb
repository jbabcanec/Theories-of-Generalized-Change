#!/usr/bin/env ruby

class GradientDescent
  def initialize(function_proc, gradient_proc)
    @func = function_proc
    @grad_func = gradient_proc
  end
  
  def optimize(initial_x, learning_rate, num_iterations)
    x = initial_x.to_f
    x_history = [x]
    
    puts "Gradient Descent Progress:"
    puts "Iteration\tx\t\tf(x)"
    puts "0\t\t#{x.round(6)}\t#{@func.call(x).round(6)}"
    
    num_iterations.times do |i|
      grad = @grad_func.call(x)
      x = x - learning_rate * grad
      x_history << x
      
      if (i + 1) % 10 == 0
        puts "#{i + 1}\t\t#{x.round(6)}\t#{@func.call(x).round(6)}"
      end
    end
    
    x_history
  end
  
  def save_optimization_path(x_history, filename)
    File.open(filename, 'w') do |file|
      file.puts "iteration,x,fx"
      
      x_history.each_with_index do |x, i|
        file.puts "#{i},#{x},#{@func.call(x)}"
      end
    end
    
    puts "Optimization path saved to #{filename}"
  end
  
  def test_learning_rates(initial_x, iterations = 20)
    puts "\nEffect of Different Learning Rates:"
    puts "Learning Rate\tFinal x\t\tFinal f(x)"
    
    [0.01, 0.1, 0.5, 0.9].each do |rate|
      history = optimize_quietly(initial_x, rate, iterations)
      final_x = history.last
      puts "#{rate}\t\t#{final_x.round(6)}\t#{@func.call(final_x).round(6)}"
    end
  end
  
  private
  
  def optimize_quietly(initial_x, learning_rate, num_iterations)
    x = initial_x.to_f
    x_history = [x]
    
    num_iterations.times do
      grad = @grad_func.call(x)
      x = x - learning_rate * grad
      x_history << x
    end
    
    x_history
  end
end

# Example functions
quadratic = ->(x) { x**2 }
quadratic_derivative = ->(x) { 2 * x }

rosenbrock_1d = ->(x) { (1 - x)**2 + 100 * (1 - x**2)**2 }
rosenbrock_1d_derivative = ->(x) { -2 * (1 - x) - 400 * x * (1 - x**2) }

noisy_quadratic = ->(x) { x**2 + 0.1 * Math.sin(20 * x) }
noisy_quadratic_derivative = ->(x) { 2 * x + 2 * Math.cos(20 * x) }

puts "Gradient Descent Optimization (Ruby version)"
puts "============================================="
puts

# Example 1: Simple quadratic
puts "Example 1: f(x) = x²"
puts "Initial x: 10.0"
puts "Learning rate: 0.1"
puts "Iterations: 50"
puts

gd1 = GradientDescent.new(quadratic, quadratic_derivative)
x_history1 = gd1.optimize(10.0, 0.1, 50)
gd1.save_optimization_path(x_history1, "quadratic_optimization_ruby.csv")

puts "\nFinal result: x = #{x_history1.last.round(6)}, f(x) = #{quadratic.call(x_history1.last).round(6)}"
puts "Theoretical minimum: x = 0, f(x) = 0"

# Convergence analysis
puts "\nConvergence Analysis:"
puts "Iteration\tx\t\tf(x)\t\tError"
(0...20).step(2) do |i|
  break if i >= x_history1.length
  x = x_history1[i]
  fx = quadratic.call(x)
  error = (x - 0).abs  # Distance from true minimum
  puts "#{i}\t\t#{x.round(6)}\t#{fx.round(6)}\t#{error.round(6)}"
end

gd1.test_learning_rates(10.0)

# Example 2: More complex function
puts "\n\nExample 2: Modified Rosenbrock (1D)"
gd2 = GradientDescent.new(rosenbrock_1d, rosenbrock_1d_derivative)
x_history2 = gd2.optimize(2.0, 0.001, 1000)
gd2.save_optimization_path(x_history2, "rosenbrock_optimization_ruby.csv")

puts "Final result: x = #{x_history2.last.round(6)}, f(x) = #{rosenbrock_1d.call(x_history2.last).round(6)}"
puts "Theoretical minimum: x = 1, f(x) = 0"

# Example 3: Multiple starting points
puts "\n\nExample 3: Function with Multiple Local Minima"
gd3 = GradientDescent.new(noisy_quadratic, noisy_quadratic_derivative)

starting_points = [-3.0, -1.0, 1.0, 3.0]
starting_points.each do |start|
  history = gd3.optimize_quietly(start, 0.05, 100)
  puts "Starting from x = #{start}, converged to x = #{history.last.round(6)}"
end

# Analysis of optimization behavior
puts "\n\nOptimization Behavior Analysis:"

# Momentum-based improvement simulation
class MomentumGradientDescent < GradientDescent
  def optimize_with_momentum(initial_x, learning_rate, momentum, num_iterations)
    x = initial_x.to_f
    velocity = 0.0
    x_history = [x]
    
    puts "Momentum Gradient Descent (β = #{momentum}):"
    puts "Iteration\tx\t\tf(x)"
    puts "0\t\t#{x.round(6)}\t#{@func.call(x).round(6)}"
    
    num_iterations.times do |i|
      grad = @grad_func.call(x)
      velocity = momentum * velocity + learning_rate * grad
      x = x - velocity
      x_history << x
      
      if (i + 1) % 10 == 0
        puts "#{i + 1}\t\t#{x.round(6)}\t#{@func.call(x).round(6)}"
      end
    end
    
    x_history
  end
end

puts "\n\nComparison: Standard vs Momentum Gradient Descent"
momentum_gd = MomentumGradientDescent.new(quadratic, quadratic_derivative)

# Standard GD
standard_history = momentum_gd.optimize_quietly(10.0, 0.05, 30)
puts "Standard GD: #{standard_history.length} steps to reach #{standard_history.last.round(6)}"

# Momentum GD
momentum_history = momentum_gd.optimize_with_momentum(10.0, 0.05, 0.9, 30)
puts "Momentum GD converged to: #{momentum_history.last.round(6)}"

puts "\nNote: Generated CSV files can be plotted using various tools for visualization."