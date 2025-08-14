import sympy as sp

def solve_ode_with_sympy():
    t = sp.Symbol('t', real=True, nonnegative=True)
    y = sp.Function('y')(t)
    
    # Define the ODE:
    # y'' + 2y' + 5y = e^(-t)*cos(2t) + DiracDelta(t - 3)
    ode = sp.Eq(
        y.diff(t, 2) + 2*y.diff(t) + 5*y,
        sp.exp(-t)*sp.cos(2*t) + sp.DiracDelta(t - 3)
    )
    
    # Solve with initial conditions y(0)=1, y'(0)=0
    sol = sp.dsolve(
        ode,
        ics={
            y.subs(t, 0): 1,
            y.diff(t).subs(t, 0): 0
        }
    )
    return sol

if __name__ == "__main__":
    solution = solve_ode_with_sympy()
    print("Solution to the ODE:")
    print(solution)
