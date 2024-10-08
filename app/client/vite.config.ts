import { defineConfig } from "vite";
import react from "@vitejs/plugin-react-swc";
import svgr from "vite-plugin-svgr";

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [react(), svgr()],
  server: {
    host: true,
    proxy: {
      "/api": {
        target: "http://localhost:2000",
        changeOrigin: true,
      },
    },
  },
});
